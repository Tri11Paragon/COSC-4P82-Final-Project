=== BEST-OF-RUN ===
              generation: 18
                   nodes: 66
                   depth: 16
                    hits: 200
TOP INDIVIDUAL:



-- #1 --
                    hits: 200
             raw fitness: 0.1004
    standardized fitness: 0.1004
        adjusted fitness: 0.9088
TREE:
 (+ (* x x)
    (+ x
       (* x
          (- (* (* x x) x)
             (* x
                (- (* (/ (sin (+ x x))
                         (* (exp (/ (exp (* x
                                            (- (* (* x x) x)
                                               (- -0.30292 x)))) 0.91093))
                            (+ (* x
                                  (- (* (* x x)
                                        (+ x 0.83823))
                                     (log (/ 0.68915
                                             (exp 0.66163)))))
                               (/ (cos (cos x))
                                  (* 0.72481
                                     (+ -0.58125 0.16027)))))) 0.16346) x))))))
TREE-equ:
y = ((x * x) + (x + (x * (((x * x) * x) - (x * (((sin((x + x)) / (exp((exp((x * (((x * x) * x) - (-0.30292 - x)))) / 0.91093)) * ((x * (((x * x) * (x + 0.83823)) - log((0.68915 / exp(0.66163))))) + (cos(cos(x)) / (0.72481 * (-0.58125 + 0.16027)))))) * 0.16346) - x))))))
