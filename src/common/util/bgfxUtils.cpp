//
//  bgfxUtils.cpp
//  AppGPUImage
//
//  Created by 董宏昌 on 2018/6/29.
//  Copyright © 2018年 董宏昌. All rights reserved.
//

#include "bgfxUtils.h"
#include <bx/bx.h>
#include <bx/file.h>
#include <bimg/decode.h>
#include "core/AGIContext.h"


// hack to fix the multiple definition link errors
#define getUniformTypeName getUniformTypeName_shaderc
#define nameToUniformTypeEnum nameToUniformTypeEnum_shaderc
#define s_uniformTypeName s_uniformTypeName_shaderc

// include original shaderc code files
#include "../../bgfx/tools/shaderc/shaderc.cpp"
#include "../../bgfx/tools/shaderc/shaderc_hlsl.cpp"
#include "../../bgfx/tools/shaderc/shaderc_glsl.cpp"

// hack bgfx for bgfxRuntimeShader
namespace bgfx
{
    bool compilePSSLShader(const Options&, uint32_t, const std::string&, bx::WriterI*)
    {
        return false;
    }

    bool compileSPIRVShader(const Options&, uint32_t, const std::string&, bx::WriterI*)
    {
        return false;
    }

    const char* getPsslPreamble()
    {
        return "";
    }
}


namespace bgfxRuntimeShader {

    /// not a real FileWriter, but a hack to redirect write() to a memory block.
    class BufferWriter : public bx::FileWriter
    {
    public:

        BufferWriter()
        {
        }

        ~BufferWriter()
        {
        }

        bool open(const bx::FilePath& _filePath, bool _append, bx::Error* _err) override
        {
            return true;
        }

        const bgfx::Memory* finalize()
        {
            if(_buffer.size() > 0)
            {
                _buffer.push_back('\0');

                const bgfx::Memory* mem = bgfx::alloc(_buffer.size());
                bx::memCopy(mem->data, _buffer.data(), _buffer.size());
                return mem;
            }

            return nullptr;
        }

        int32_t write(const void* _data, int32_t _size, bx::Error* _err) override
        {
            const char* data = (const char*)_data;
            _buffer.insert(_buffer.end(), data, data+_size);
            return _size;
        }

    private:
        BX_ALIGN_DECL(16, uint8_t) m_internal[64];
        typedef std::vector<uint8_t> Buffer;
        Buffer _buffer;
    };

    const bgfx::Memory* compileShader(bgfxUtils::ShaderType type, const char* filePath, const char* defines, const char* varyingPath, const char* profile)
    {
        bgfx::Options options;

        options.inputFilePath = filePath;
        options.shaderType = type;

        // set platform
#if BX_PLATFORM_LINUX
        options.platform = "linux";
#elif BX_PLATFORM_WINDOWS
        options.platform = "windows";
#elif BX_PLATFORM_ANDROID
        options.platform = "android";
#elif BX_PLATFORM_EMSCRIPTEN
        options.platform = "asm.js";
#elif BX_PLATFORM_IOS
        options.platform = "ios";
#elif BX_PLATFORM_OSX
        options.platform = "osx";
#endif

        // set profile
        if(profile)
        {
            // user profile
            options.profile = profile;
        }
        else
        {
            // set default profile for current running renderer.

            bgfx::RendererType::Enum renderType = bgfx::getRendererType();

            switch(renderType)
            {
                default:
                case bgfx::RendererType::Noop:         //!< No rendering.
                    break;
                case bgfx::RendererType::Direct3D9:    //!< Direct3D 9.0
                {
                    if(type == 'v')
                        options.profile = "vs_3_0";
                    else if(type == 'f')
                        options.profile = "ps_3_0";
                    else if(type == 'c')
                        options.profile = "ps_3_0";
                }
                    break;
                case bgfx::RendererType::Direct3D11:   //!< Direct3D 11.0
                {
                    if(type == 'v')
                        options.profile = "vs_4_0";
                    else if(type == 'f')
                        options.profile = "ps_4_0";
                    else if(type == 'c')
                        options.profile = "cs_5_0";
                }
                    break;
                case bgfx::RendererType::Direct3D12:   //!< Direct3D 12.0
                {
                    if(type == 'v')
                        options.profile = "vs_5_0";
                    else if(type == 'f')
                        options.profile = "ps_5_0";
                    else if(type == 'c')
                        options.profile = "cs_5_0";
                }
                    break;
                case bgfx::RendererType::Gnm:          //!< GNM
                    break;
                case bgfx::RendererType::Metal:        //!< Metal
                {
                    options.profile = "metal";
                }
                    break;
                case bgfx::RendererType::OpenGLES:     //!< OpenGL ES 2.0+
                    break;
                case bgfx::RendererType::OpenGL:       //!< OpenGL 2.1+
                {
                    if(type == 'v' || type == 'f')
                        options.profile = "120";
                    else if(type == 'c')
                        options.profile = "430";
                }
                    break;
                case bgfx::RendererType::Vulkan:       //!< Vulkan
                    break;
            };
        }


        // include current dir
        std::string dir;
        {
            const char* base = bgfx::baseName(filePath);

            if (base != filePath)
            {
                dir.assign(filePath, base-filePath);
                options.includeDirs.push_back(dir.c_str());
            }
        }

        // set defines
        while (NULL != defines
               &&    '\0'  != *defines)
        {
            defines = bx::strws(defines);
            const char* eol = bx::strFind(defines, ';');
            if (NULL == eol)
            {
                eol = defines + bx::strLen(defines);
            }
            std::string define(defines, eol);
            options.defines.push_back(define.c_str() );
            defines = ';' == *eol ? eol+1 : eol;
        }


        // set varyingdef
        std::string defaultVarying = dir + "varying.def.sc";
        const char* varyingdef = varyingPath ? varyingPath : defaultVarying.c_str();
        bgfx::File attribdef(varyingdef);
        const char* parse = attribdef.getData();
        if (NULL != parse
            &&  *parse != '\0')
        {
            options.dependencies.push_back(varyingdef);
        }
        else
        {
            fprintf(stderr, "ERROR: Failed to parse varying def file: \"%s\" No input/output semantics will be generated in the code!\n", varyingdef);
            return nullptr;
        }



        // read shader source file
        bx::FileReader reader;
        if (!bx::open(&reader, filePath) )
        {
            fprintf(stderr, "Unable to open file '%s'.\n", filePath);
            return nullptr;
        }

        // add padding
        const size_t padding = 4096;
        uint32_t size = (uint32_t)bx::getSize(&reader);
        char* data = new char[size+padding+1];
        size = (uint32_t)bx::read(&reader, data, size);

        if (data[0] == '\xef'
            &&  data[1] == '\xbb'
            &&  data[2] == '\xbf')
        {
            bx::memMove(data, &data[3], size-3);
            size -= 3;
        }

        // Compiler generates "error X3000: syntax error: unexpected end of file"
        // if input doesn't have empty line at EOF.
        data[size] = '\n';
        bx::memSet(&data[size+1], 0, padding);
        bx::close(&reader);


        std::string commandLineComment = "// shaderc command line:\n";

        // compile shader.

        BufferWriter writer;
        if ( bgfx::compileShader(attribdef.getData(), commandLineComment.c_str(), data, size, options, &writer) )
        {
            // this will copy the compiled shader data to a memory block and return mem ptr
            return writer.finalize();
        }

        return nullptr;
    }

    bgfx::ShaderHandle createShader(bgfxUtils::ShaderType type,
                                    const char *filePath,
                                    const char *defines,
                                    const char *varyingPath,
                                    const char *profile) {
        auto shaderMem = compileShader(type, filePath, defines, varyingPath, profile);
        auto shader = bgfx::createShader(shaderMem);

        return shader;
    }
}


namespace bgfxUtils {
    bimg::ImageContainer *loadImage(const char *filePath) {
        auto bxFilePath = bx::FilePath(filePath);

        auto error = new bx::Error();
        auto fileReader = new bx::FileReader();
        fileReader->open(bxFilePath, error);

        auto alloctor = AGIContext::sharedContext()->getDefaultAllocator();

        uint32_t fileSize = (uint32_t)bx::getSize(fileReader);
        void *data = BX_ALLOC(alloctor, fileSize);
        bx::read(fileReader, data, fileSize);
        bx::close(fileReader);
        delete error;
        delete fileReader;

        auto ic = bimg::imageParse(alloctor, data, fileSize);
        BX_FREE(alloctor, data);

        return ic;
    }

    bgfx::TextureHandle loadTextureFromImageContainer(bimg::ImageContainer* ic) {
        auto mem = bgfx::copy(ic->m_data, ic->m_size);
        auto textureFormat = bgfx::TextureFormat::Enum(ic->m_format);
        bool hasMips = 1 < ic->m_numMips;
        auto textureHandle = bgfx::createTexture2D(ic->m_width, ic->m_height, hasMips, ic->m_numLayers, textureFormat, BGFX_TEXTURE_NONE, mem);

        return textureHandle;
    }

    bgfx::TextureHandle loadTexture(const char *filePath) {
        auto ic = bgfxUtils::loadImage(filePath);

        auto textureHandle = loadTextureFromImageContainer(ic);
        bimg::imageFree(ic);

        return textureHandle;
    }

    bgfx::TextureHandle loadTexture(const void* data, uint32_t size) {
        auto alloctor = AGIContext::sharedContext()->getDefaultAllocator();
        auto ic = bimg::imageParse(alloctor, data, size);

        auto textureHandle = loadTextureFromImageContainer(ic);
        bimg::imageFree(ic);

        return textureHandle;
    }

    bgfx::ShaderHandle createShader(ShaderType type,
                                    const char *filePath,
                                    const char *defines,
                                    const char *varyingPath,
                                    const char *profile) {
        return bgfxRuntimeShader::createShader(type, filePath, defines, varyingPath, profile);
    }

    cv::Mat generateCVMatFromBuffer(bgfx::Memory* mem, bgfx::TextureFormat::Enum format, int width, int height)
    {
        int matFormat = 0;
        if (format == bgfx::TextureFormat::Enum::RGBA8)
        {
            matFormat = CV_8UC4;
        }
        else if (format == bgfx::TextureFormat::Enum::R8)
        {
            matFormat = CV_8UC1;
        }

        cv::Mat mat = cv::Mat(height, width, matFormat);
		memcpy(mat.data, mem->data, mem->size);

        return mat;
    }

    bgfx::TextureHandle generateTextureFromBuffer(bgfx::Memory* mem, bgfx::TextureFormat::Enum format, int width, int height)
    {
        auto copyMem = bgfx::copy(mem->data, mem->size);
        auto textureFormat = format;
        bool hasMips = false;
        auto textureHandle = bgfx::createTexture2D(width, height, hasMips, 1, textureFormat, BGFX_TEXTURE_NONE, copyMem);

        return textureHandle;
    }
}

