;; Emacs C++ local (only under this directory) settings 

((nil . ((flycheck-clang-language-standard . "c++17") ; このプロジェクトはC++11に対応している
         (flycheck-clang-include-path . ("."          ; インクルードパスを設定する
                                         "TGV"
                                         )))))
