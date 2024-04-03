=== BEST-OF-RUN ===
              generation: 42
                   nodes: 39
                   depth: 6
                    hits: 780
TOP INDIVIDUAL:



-- #1 --
                    hits: 780
             raw fitness: 780.0000
    standardized fitness: 780.0000
        adjusted fitness: 0.9987
TREE:
 (- (log (/ 0.35790 minor))
    (- (exp (* (* (* minor extent) extent)
               (- (/ perimeter major)
                  (exp eccentricity))))
       (* (log (- (+ 0.41684 major)
                  (- major major)))
          (- (log (exp 0.91292))
             (* (/ eccentricity perimeter)
                (* major -0.72939))))))
TREE-equ:
y = (log((0.35790 / minor)) - (exp((((minor * extent) * extent) * ((perimeter / major) - exp(eccentricity)))) - (log(((0.41684 + major) - (major - major))) * (log(exp(0.91292)) - ((eccentricity / perimeter) * (major * -0.72939))))))
