=== BEST-OF-RUN ===
              generation: 2
                   nodes: 47
                   depth: 6
                    hits: 900
TOP INDIVIDUAL:



-- #1 --
                    hits: 900
             raw fitness: 900.0000
    standardized fitness: 900.0000
        adjusted fitness: 0.9989
TREE:
 (- (* (exp (/ (+ (* area convex)
                  (- area area))
               (log (/ extent perimeter))))
       (- (* (/ (+ major area)
                (+ major extent))
             (/ (* major major)
                (exp area)))
          (log (- (+ perimeter minor)
                  (log extent)))))
    (exp (* (- 0.25550 perimeter)
            (log (+ extent
                    (log minor))))))
TREE-equ:
y = ((exp((((area * convex) + (area - area)) / log((extent / perimeter)))) * ((((major + area) / (major + extent)) * ((major * major) / exp(area))) - log(((perimeter + minor) - log(extent))))) - exp(((0.25550 - perimeter) * log((extent + log(minor))))))
