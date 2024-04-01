=== BEST-OF-RUN ===
              generation: 20
                   nodes: 37
                   depth: 10
                    hits: 200
TOP INDIVIDUAL:



-- #1 --
                    hits: 200
             raw fitness: 0.7401
    standardized fitness: 0.7401
        adjusted fitness: 0.5747
TREE:
 (- (* x
       (- (* (* x x)
             (+ (/ x x) x))
          (+ (+ -0.19484
                (/ (* x
                      (log (* (- x x)
                              (+ 0.16530
                                 (sin (sin (* x x)))))))
                   (* x
                      (- (* (* x x)
                            (+ (/ x x) x))
                         (+ (log (+ 0.16530
                                    (- x
                                       (+ x
                                          (/ x x)))))
                            (- -0.80591 x))))))
             (- -0.80591 x)))) -0.00404)
TREE-equ:
y = ((x * (((x * x) * ((x / x) + x)) - ((-0.19484 + ((x * log(((x - x) * (0.16530 + sin(sin((x * x))))))) / (x * (((x * x) * ((x / x) + x)) - (log((0.16530 + (x - (x + (x / x))))) + (-0.80591 - x)))))) + (-0.80591 - x)))) - -0.00404)
