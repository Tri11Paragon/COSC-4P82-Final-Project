=== BEST-OF-RUN ===
              generation: 39
                   nodes: 83
                   depth: 17
                    hits: 5
TOP INDIVIDUAL:



-- #1 --
                    hits: 5
             raw fitness: 44.1929
    standardized fitness: 44.1929
        adjusted fitness: 0.0221
TREE:
 (* x
    (+ (+ (* (* x x)
             (+ x 0.95783))
          (- (log (+ x x))
             (- (* (cos (sin (* x -0.65293)))
                   (cos (exp (cos x)))) x)))
       (- (* -0.52746 -0.52845)
          (* (* (- (cos x)
                   (log (+ x x)))
                (cos (* (* x x)
                        (+ x 0.95783))))
             (* (- (log (* x
                           (- (log (- 0.73343
                                      (+ (+ (* (log (sin (* -0.52746 -0.52845)))
                                               (+ x 0.46729)) x)
                                         (/ (* -0.52746 -0.52845) x))))
                              (- 0.73343 x))))
                   (log -0.52746))
                (- x x))))))
TREE-equ:
y = (x * ((((x * x) * (x + 0.95783)) + (log((x + x)) - ((cos(sin((x * -0.65293))) * cos(exp(cos(x)))) - x))) + ((-0.52746 * -0.52845) - (((cos(x) - log((x + x))) * cos(((x * x) * (x + 0.95783)))) * ((log((x * (log((0.73343 - (((log(sin((-0.52746 * -0.52845))) * (x + 0.46729)) + x) + ((-0.52746 * -0.52845) / x)))) - (0.73343 - x)))) - log(-0.52746)) * (x - x))))))
