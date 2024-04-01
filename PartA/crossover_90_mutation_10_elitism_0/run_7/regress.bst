=== BEST-OF-RUN ===
              generation: 40
                   nodes: 116
                   depth: 16
                    hits: 0
TOP INDIVIDUAL:



-- #1 --
                    hits: 0
             raw fitness: 3004.6946
    standardized fitness: 3004.6946
        adjusted fitness: 0.0003
TREE:
 (log (* (+ (+ x
               (exp (+ (log (+ x
                               (log (/ (exp (log x))
                                       (/ (exp (- (sin (log (cos x)))
                                                  (exp 0.56982)))
                                          (cos (- (* 0.27165 0.48717)
                                                  (log (exp (* x 0.51767))))))))))
                       (- (- (log (/ (log x) -0.11285))
                             (* (sin (* (+ (cos x)
                                           (+ (- -0.13433 x)
                                              (/ (cos -0.96676)
                                                 (log (/ x -0.68555)))))
                                        (* x -0.81129)))
                                (exp x)))
                          (* 0.05272 x))))) x)
         (- (/ (exp (/ x
                       (/ (* (* -0.44219 0.35652) -0.41758)
                          (+ (- (- 0.53575
                                   (- (sin x)
                                      (sin (* x -0.81129))))
                                (log (log 0.52374))) x))))
               (exp x))
            (sin (- (- 0.53575
                       (- (sin (- 0.53575
                                  (- (sin x)
                                     (sin (* x -0.81129)))))
                          (sin (* x -0.81129))))
                    (sin -0.38775))))))
TREE-equ:
y = log((((x + exp((log((x + log((exp(log(x)) / (exp((sin(log(cos(x))) - exp(0.56982))) / cos(((0.27165 * 0.48717) - log(exp((x * 0.51767)))))))))) + ((log((log(x) / -0.11285)) - (sin(((cos(x) + ((-0.13433 - x) + (cos(-0.96676) / log((x / -0.68555))))) * (x * -0.81129))) * exp(x))) - (0.05272 * x))))) + x) * ((exp((x / (((-0.44219 * 0.35652) * -0.41758) / (((0.53575 - (sin(x) - sin((x * -0.81129)))) - log(log(0.52374))) + x)))) / exp(x)) - sin(((0.53575 - (sin((0.53575 - (sin(x) - sin((x * -0.81129))))) - sin((x * -0.81129)))) - sin(-0.38775))))))
