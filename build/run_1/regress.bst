=== BEST-OF-RUN ===
              generation: 3
                   nodes: 44
                   depth: 9
                    hits: 881
TOP INDIVIDUAL:



-- #1 --
                    hits: 881
             raw fitness: 881.0000
    standardized fitness: 881.0000
        adjusted fitness: 0.9989
TREE:
 (- (exp (/ (exp (/ (- convex major)
                    (exp area)))
            (exp (log -0.43080))))
    (- (* (log (* (exp major)
                  (exp (exp (* (+ minor eccentricity)
                               (* eccentricity -0.70567))))))
          (- (log (* area minor))
             (* (* 0.07897 major) eccentricity)))
       (exp (log (exp (- perimeter area))))))
TREE-equ:
y = (exp((exp(((convex - major) / exp(area))) / exp(log(-0.43080)))) - ((log((exp(major) * exp(exp(((minor + eccentricity) * (eccentricity * -0.70567)))))) * (log((area * minor)) - ((0.07897 * major) * eccentricity))) - exp(log(exp((perimeter - area))))))
