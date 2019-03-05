;; Emacs C++ local (only under this directory) settings 


;;; CODE:
((nil .
      ((flycheck-clang-language-standard . "c++1z") 
       (flycheck-clang-include-path . ("~/work/opencv/TGV/"
                                       ))
       (flycheck-gcc-language-standard . "c++1z")
       (flycheck-gcc-include-path . ("~/work/opencv/TGV/"
                                     )))
      ;; ((company-clang-arguments . ("-I/home/kitamura/work/opencv/TGV/"
      ;;                              )))
      ))
