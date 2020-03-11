;; Emacs C++ local (only under this directory) settings 

;;; CODE:
((c++-mode .
      ((flycheck-clang-language-standard . "c++17")
       (flycheck-clang-include-path . (
                                       "/usr/local/include/opencv4"
                                       "/home/kitamura/work/ImageProcessing/ext"
                                       "/home/kitamura/work/ImageProcessing/ext/imgui"
                                       "/home/kitamura/work/ImageProcessing/ext/fmt/include"
                                       "/home/kitamura/work/ImageProcessing/ext/cereal/include"
                                       "/home/kitamura/work/ImageProcessing/ext/nameof/include"
                                       "/home/kitamura/work/ImageProcessing/ext/imgui/examples"
                                       "/home/kitamura/work/ImageProcessing/ext/imgui/examples/libs/gl3w"
                                       "/usr/include/eigen3"
                                       "/home/kitamura/work/ImageProcessing/src"
                                       ))
      )))


;; ((irony-cdb-json-add-compile-commands-path "./" "../build/opencv/compile_commands.json"))
(irony-cdb-json-add-compile-commands-path "./" "../build/ImageProcessing/compile_commands.json")
