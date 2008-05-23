(when 1
(setq licnotice-var-progname "podcastfs")
(setq licnotice-var-whatitdoes "a simple fuse filesystem for mounting podcast")
(setq licnotice-var-author "Kristian Rumberg")
(setq licnotice-var-email "kristianrumberg@gmail.com")
(setq licnotice-var-license "ISC")
(licnotice-enable)

(setq compile-command "cd ../bin/ && make")

(mapc '(lambda(l) (mapc 'find-file l)) (mapcar 'file-expand-wildcards '("*.c" "*.h")))

)