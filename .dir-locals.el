;;; Directory Local Variables            -*- no-byte-compile: t -*-
;;; For more information see (info "(emacs) Directory Variables")
;;; .dir-locals.el
((c++-mode
  . ((flycheck-gcc-language-standard . "c++20")
     (flycheck-clang-language-standard . "c++20")
     (flycheck-disabled-checkers . (c/c++-gcc))
     (eglot-workspace-configuration
      . ((:clangd . (:compileFlags ["-std=c++20"])))))))
