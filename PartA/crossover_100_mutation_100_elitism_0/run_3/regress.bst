=== BEST-OF-RUN ===
              generation: 50
                   nodes: 43
                   depth: 11
                    hits: 0
TOP INDIVIDUAL:



-- #1 --
                    hits: 0
             raw fitness: 1801.6112
    standardized fitness: 1801.6112
        adjusted fitness: 0.0006
TREE:
 (- (* (log (+ (exp x)
               (* x
                  (+ 0.90503 x))))
       (* (- (cos (- x
                     (* (exp (/ x x))
                        (+ 0.20909
                           (log x)))))
             (- (/ x -0.24597)
                (- x
                   (+ (exp (* x
                              (cos (log x))))
                      (sin x))))) x))
    (sin -0.24597))
TREE-equ:
y = ((log((exp(x) + (x * (0.90503 + x)))) * ((cos((x - (exp((x / x)) * (0.20909 + log(x))))) - ((x / -0.24597) - (x - (exp((x * cos(log(x)))) + sin(x))))) * x)) - sin(-0.24597))
