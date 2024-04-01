=== BEST-OF-RUN ===
              generation: 48
                   nodes: 91
                   depth: 16
                    hits: 6
TOP INDIVIDUAL:



-- #1 --
                    hits: 6
             raw fitness: 66.3483
    standardized fitness: 66.3483
        adjusted fitness: 0.0148
TREE:
 (- (* x
       (- (* (* x x) x)
          (/ x
             (/ -0.93562
                (+ (cos (log -0.79526)) x)))))
    (* (cos (/ x
               (* x
                  (- (* (/ -0.93562
                           (+ (cos (cos x)) x))
                        (- (cos x)
                           (/ x
                              (/ -0.93562
                                 (+ (sin (log (exp (- (cos -0.34761)
                                                      (cos -0.70143))))) x)))))
                     (* (cos (+ (cos (log -0.79526)) x))
                        (- (sin (* (sin (* x 0.50352)) -0.43047))
                           (sin (* x 0.32173))))))))
       (- (- (+ -0.25388 0.36742)
             (sin (- x
                     (* (+ (- 0.19344 x) x)
                        (+ (+ -0.36588 x) -0.73379)))))
          (sin (sin x)))))
TREE-equ:
y = ((x * (((x * x) * x) - (x / (-0.93562 / (cos(log(-0.79526)) + x))))) - (cos((x / (x * (((-0.93562 / (cos(cos(x)) + x)) * (cos(x) - (x / (-0.93562 / (sin(log(exp((cos(-0.34761) - cos(-0.70143))))) + x))))) - (cos((cos(log(-0.79526)) + x)) * (sin((sin((x * 0.50352)) * -0.43047)) - sin((x * 0.32173)))))))) * (((-0.25388 + 0.36742) - sin((x - (((0.19344 - x) + x) * ((-0.36588 + x) + -0.73379))))) - sin(sin(x)))))
