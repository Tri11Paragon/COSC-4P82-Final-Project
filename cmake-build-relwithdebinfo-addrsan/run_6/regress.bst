=== BEST-OF-RUN ===
              generation: 33
                   nodes: 24
                   depth: 7
                    hits: 896
TOP INDIVIDUAL:



-- #1 --
                    hits: 896
             raw fitness: 896.0000
    standardized fitness: 896.0000
        adjusted fitness: 0.9989
TREE:
 (log (/ (+ major perimeter)
         (* (+ convex
               (* (exp major)
                  (- extent
                     (log major))))
            (+ (exp (/ perimeter eccentricity))
               (exp (- minor perimeter))))))
TREE-equ:
y = log(((major + perimeter) / ((convex + (exp(major) * (extent - log(major)))) * (exp((perimeter / eccentricity)) + exp((minor - perimeter))))))
