=== BEST-OF-RUN ===
              generation: 4
                   nodes: 23
                   depth: 7
                    hits: 921
TOP INDIVIDUAL:



-- #1 --
                    hits: 921
             raw fitness: 921.0000
    standardized fitness: 921.0000
        adjusted fitness: 0.9989
TREE:
 (- (exp (/ (log minor)
            (exp eccentricity)))
    (/ (* perimeter eccentricity)
       (exp (/ (* (+ (+ perimeter 0.29261) perimeter)
                  (- minor perimeter)) perimeter))))
TREE-equ:
y = (exp((log(minor) / exp(eccentricity))) - ((perimeter * eccentricity) / exp(((((perimeter + 0.29261) + perimeter) * (minor - perimeter)) / perimeter))))
