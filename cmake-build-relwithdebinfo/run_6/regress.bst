=== BEST-OF-RUN ===
              generation: 0
                   nodes: 24
                   depth: 5
                    hits: 668
TOP INDIVIDUAL:



-- #1 --
                    hits: 668
             raw fitness: 668.0000
    standardized fitness: 668.0000
        adjusted fitness: 0.9985
TREE:
 (* (/ (- (log (- major minor))
          (log (+ eccentricity minor)))
       (log (- (* minor area)
               (/ area minor))))
    (exp (exp (exp (log major)))))
TREE-equ:
y = (((log((major - minor)) - log((eccentricity + minor))) / log(((minor * area) - (area / minor)))) * exp(exp(exp(log(major)))))
