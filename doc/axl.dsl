<!DOCTYPE style-sheet PUBLIC "-//James Clark//DTD DSSSL Style Sheet//EN" [
<!ENTITY % html "IGNORE">
<![%html;[
<!ENTITY % print "IGNORE">
<!ENTITY docbook.dsl PUBLIC "-//Norman Walsh//DOCUMENT DocBook HTML Stylesheet//EN" CDATA dsssl>
]]>
<!ENTITY % print "INCLUDE">
<![%print;[
<!ENTITY docbook.dsl PUBLIC "-//Norman Walsh//DOCUMENT DocBook Print Stylesheet//EN" CDATA dsssl>
]]>
]>
<style-sheet>
<style-specification id="print" use="docbook">
<style-specification-body> 


(define %two-side% 
  ;; Is two-sided output being produced?
    #f)
;; customize the print stylesheet
(define %generate-book-titlepage%	#t)
(define %generate-part-titlepage% 	#t)
(define %generate-partintro-on-titlepage%	#t)
(define %generate-reference-titlepage% 	#t)

(define %generate-article-titlepage% 	#t)
    

(define %generate-reference-toc%	#t)
    
(define %generate-part-toc%		#t)

(define %paper-type% "A4"
    ;; "USletter"
    )

;; Width of left margin
(define %left-margin% 4pi)

;; Width of right margin
(define %right-margin% 3pi)

;; Width of top margin
(define %top-margin% 6pi)

;; Width of bottom margin
(define %bottom-margin% 3pi)

(define %page-n-columns% 1)
(define %page-column-sep% 3pi)

(define (toc-depth nd) 3)

;;(define %body-font-family% 
  ;; The font family used in body text
;;    "Helvetica")
</style-specification-body>
</style-specification>


<style-specification id="html" use="docbook">
<style-specification-body> 

(define %root-filename% "index")	;; name for the root html file
(define %html-ext% ".html")		;; default extension for html output files
(define %html-prefix% "")               ;; prefix for all filenames generated (except root)
(define %use-id-as-filename% #t)        ;; uses ID value, if present, as filename
                                        ;;   otherwise a code is used to indicate level
                                        ;;   of chunk, and general element number
                                        ;;   (nth element in the document)
(define use-output-dir #f)              ;; output in separate directory?
(define %output-dir% "HTML")            ;; if output in directory, it's called HTML

; === HTML settings ===
(define %html-pubid% "-//W3C//DTD HTML 4.01 Transitional//EN") ;; Nearly true :-(
(define %html40% #t)

; === Media objects ===
(define preferred-mediaobject-extensions  ;; this magic allows to use different graphical
  (list "png" "jpg" "jpeg"))		;;   formats for printing and putting online
(define acceptable-mediaobject-extensions
  (list "bmp" "gif" "eps" "epsf" "avi" "mpg" "mpeg" "qt"))
(define preferred-mediaobject-notations
  (list "PNG" "JPG" "JPEG"))
(define acceptable-mediaobject-notations
  (list "EPS" "BMP" "GIF" "linespecific"))                                                                                                    
; === Rendering ===
(define %admon-graphics% #t)		;; use symbols for Caution|Important|Note|Tip|Warning

; === Books only ===
(define %generate-book-titlepage% #t)
(define %generate-book-toc% #t)
(define ($generate-chapter-toc$) #t)	;; never generate a chapter TOC in books


(define %chapter-autolabel% #t)
(define %section-autolabel% #t)
(define (toc-depth nd) 3)
(define %stylesheet-version% "Modular DocBook HTML AXLDoc Stylesheet version 1.0")

(define %stylesheet% "axldoc.css");
<!--
(define %body-attr% (list (list "bgcolor" "silver") (list "text" "navy")) )
-->

</style-specification-body>
</style-specification>
<external-specification id="docbook" document="docbook.dsl">
</style-sheet>
