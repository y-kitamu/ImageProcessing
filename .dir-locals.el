;; Emacs C++ local (only under this directory) settings 


;;; CODE:
((c++-mode .
      ((flycheck-clang-language-standard . "c++1z")
       (flycheck-clang-include-path . (
                                       "~/work/opencv/TGV/"
                                       "/home/kitamura/work/opencv/ext/"
                                       "/home/kitamura/work/opencv/ext/eigen"
                                       ))
      )))

((irony-cdb-json-add-compile-commands-path "./" "../build/opencv/compile_commands.json"))

