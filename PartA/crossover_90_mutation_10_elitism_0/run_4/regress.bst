=== BEST-OF-RUN ===
              generation: 50
                   nodes: 125
                   depth: 16
                    hits: 1
TOP INDIVIDUAL:



-- #1 --
                    hits: 1
             raw fitness: 518.6200
    standardized fitness: 518.6200
        adjusted fitness: 0.0019
TREE:
 (- (- (- (- (- (+ (- (exp (/ x
                              (/ x x)))
                      (cos (sin 0.64610)))
                   (- (/ x 0.46723)
                      (+ (sin (sin -0.28163))
                         (* (sin 0.64610)
                            (/ x -0.11075)))))
                (sin (log (- x
                             (cos (- (cos (/ x -0.11075))
                                     (* (exp (cos (/ x x)))
                                        (+ (- (cos (+ x -0.28163)) x)
                                           (exp (exp (/ x
                                                        (/ x x))))))))))))
             (sin (- (cos (exp -0.15704))
                     (- (cos (+ -0.71630 0.01312)) x)))) x)
       (sin x))
    (* (+ (+ (- (cos (- x -0.94651))
                (exp (exp 0.57537)))
             (exp (cos (cos (+ (+ (exp (exp (exp (exp 0.57537))))
                                  (- (* x 0.55323)
                                     (exp (/ x
                                             (/ x x)))))
                               (log x))))))
          (exp (exp (log x))))
       (+ (- (cos (+ x -0.28163))
             (exp (exp 0.57537)))
          (exp (cos (/ x x))))))
TREE-equ:
y = (((((((exp((x / (x / x))) - cos(sin(0.64610))) + ((x / 0.46723) - (sin(sin(-0.28163)) + (sin(0.64610) * (x / -0.11075))))) - sin(log((x - cos((cos((x / -0.11075)) - (exp(cos((x / x))) * ((cos((x + -0.28163)) - x) + exp(exp((x / (x / x)))))))))))) - sin((cos(exp(-0.15704)) - (cos((-0.71630 + 0.01312)) - x)))) - x) - sin(x)) - ((((cos((x - -0.94651)) - exp(exp(0.57537))) + exp(cos(cos(((exp(exp(exp(exp(0.57537)))) + ((x * 0.55323) - exp((x / (x / x))))) + log(x)))))) + exp(exp(log(x)))) * ((cos((x + -0.28163)) - exp(exp(0.57537))) + exp(cos((x / x))))))
