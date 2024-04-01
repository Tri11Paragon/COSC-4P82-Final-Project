=== BEST-OF-RUN ===
              generation: 8
                   nodes: 30
                   depth: 10
                    hits: 888
TOP INDIVIDUAL:



-- #1 --
                    hits: 888
             raw fitness: 888.0000
    standardized fitness: 888.0000
        adjusted fitness: 0.9989
TREE:
 (* eccentricity
    (/ extent
       (- extent
          (log (+ (* (+ (exp major)
                        (/ (* minor minor)
                           (log area)))
                     (- area
                        (/ (exp perimeter)
                           (/ convex
                              (exp minor)))))
                  (* eccentricity eccentricity))))))
TREE-equ:
y = (eccentricity * (extent / (extent - log((((exp(major) + ((minor * minor) / log(area))) * (area - (exp(perimeter) / (convex / exp(minor))))) + (eccentricity * eccentricity))))))
