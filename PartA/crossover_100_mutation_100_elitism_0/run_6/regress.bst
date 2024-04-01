=== BEST-OF-RUN ===
              generation: 45
                   nodes: 58
                   depth: 17
                    hits: 200
TOP INDIVIDUAL:



-- #1 --
                    hits: 200
             raw fitness: 1.0252
    standardized fitness: 1.0252
        adjusted fitness: 0.4938
TREE:
 (+ (- (* (* x x)
          (* x x))
       (sin 0.72466))
    (+ (* x
          (log (exp x)))
       (+ x
          (+ (+ 0.91458
                (* x
                   (+ (sin (sin (/ x
                                   (* (* (+ (cos 0.63359)
                                            (exp (+ (* (sin 0.05858)
                                                       (exp x))
                                                    (exp (exp 0.96568))))) x)
                                      (/ (exp -0.93321) 0.19571)))))
                      (* x
                         (log (exp x))))))
             (+ -0.97154
                (cos 0.77457))))))
TREE-equ:
y = ((((x * x) * (x * x)) - sin(0.72466)) + ((x * log(exp(x))) + (x + ((0.91458 + (x * (sin(sin((x / (((cos(0.63359) + exp(((sin(0.05858) * exp(x)) + exp(exp(0.96568))))) * x) * (exp(-0.93321) / 0.19571))))) + (x * log(exp(x)))))) + (-0.97154 + cos(0.77457))))))
