(when 1
(let (dir)
  (setq dir (comint-directory "."))
  (setq dir (concat (substring dir 0 (- (length dir) 3)) "bin"))
  (setq compile-command (concat "cd " dir " && make"))
  (mapc '(lambda(l) (mapc 'find-file l)) (mapcar 'file-expand-wildcards '("*.c" "*.h")))
  )
(setq licnotice-var-progname "podcastfs")
(setq licnotice-var-whatitdoes "a simple fuse filesystem for mounting podcasts")
(setq licnotice-var-author "Kristian Rumberg")
(setq licnotice-var-email "kristianrumberg@gmail.com")
(setq licnotice-var-license "ISC")
(licnotice-enable)
)


