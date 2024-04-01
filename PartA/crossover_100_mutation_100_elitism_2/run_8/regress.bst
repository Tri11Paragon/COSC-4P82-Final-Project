=== BEST-OF-RUN ===
              generation: 15
                   nodes: 66
                   depth: 15
                    hits: 200
TOP INDIVIDUAL:



-- #1 --
                    hits: 200
             raw fitness: 0.5303
    standardized fitness: 0.5303
        adjusted fitness: 0.6535
TREE:
 (* (* (+ (* x x)
          (/ x x))
       (- (sin (exp (/ (* 0.24005 x)
                       (+ (- (* (exp (sin -0.16173)) x)
                             (/ x 0.07147))
                          (* (* (cos (exp -0.91503))
                                (* (cos -0.56649)
                                   (sin -0.51065)))
                             (* (exp (/ (* 0.24005 x)
                                        (+ (- (cos -0.00366)
                                              (/ x 0.07147))
                                           (- (sin (* x x))
                                              (- -0.16814 x)))))
                                (* (exp (sin -0.16173)) x)))))))
          (- -0.16814 x))) x)
TREE-equ:
y = ((((x * x) + (x / x)) * (sin(exp(((0.24005 * x) / (((exp(sin(-0.16173)) * x) - (x / 0.07147)) + ((cos(exp(-0.91503)) * (cos(-0.56649) * sin(-0.51065))) * (exp(((0.24005 * x) / ((cos(-0.00366) - (x / 0.07147)) + (sin((x * x)) - (-0.16814 - x))))) * (exp(sin(-0.16173)) * x))))))) - (-0.16814 - x))) * x)
