rm -rf build
mkdir build
cd build
emcmake cmak ../
emmake make -j8 && emcc ../src/wasm.c ./liblvgl_demos.a ./lib/liblvgl.a -o lvgl.html -s USE_SDL=2 && emrun lvgl.html