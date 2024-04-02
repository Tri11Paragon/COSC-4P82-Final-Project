=== BEST-OF-RUN ===
              generation: 4
                   nodes: 27
                   depth: 7
                    hits: 790
TOP INDIVIDUAL:



-- #1 --
                    hits: 790
             raw fitness: 790.0000
    standardized fitness: 790.0000
        adjusted fitness: 0.9987
TREE:
 (* (exp (exp (log (+ (- perimeter eccentricity)
                      (log eccentricity)))))
    (log (log (+ (/ (/ major minor)
                    (+ (log eccentricity) eccentricity))
                 (+ (- eccentricity eccentricity)
                    (log eccentricity))))))
TREE-equ:
y = (exp(exp(log(((perimeter - eccentricity) + log(eccentricity))))) * log(log((((major / minor) / (log(eccentricity) + eccentricity)) + ((eccentricity - eccentricity) + log(eccentricity))))))
