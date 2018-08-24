
projen:
	# fix ln -s error
	rm ./src/common_deps/opencv/ios/opencv2.framework/Resources
	cd ./src/common_deps/opencv/ios/opencv2.framework && ln -s ./Versions/A/Resources ./Resources

	rm ./src/common_deps/opencv/ios/opencv2.framework/opencv2
	cd ./src/common_deps/opencv/ios/opencv2.framework && ln -s ./Versions/A/opencv2 ./opencv2

	rm ./src/common_deps/opencv/ios/opencv2.framework/Versions/Current
	cd ./src/common_deps/opencv/ios/opencv2.framework/Versions && ln -s ./A ./Current

	# generate bgfx projects
	cd ./src/common_deps/bgfx && $(MAKE) projgen
