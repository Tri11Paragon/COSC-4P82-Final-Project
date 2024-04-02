=== BEST-OF-RUN ===
              generation: 0
                   nodes: 64
                   depth: 6
                    hits: 691
TOP INDIVIDUAL:



-- #1 --
                    hits: 691
             raw fitness: 691.0000
    standardized fitness: 691.0000
        adjusted fitness: 0.9986
TREE:
 (/ (+ (exp (- (exp (+ major eccentricity))
               (* (/ convex perimeter)
                  (/ convex eccentricity))))
       (+ (/ (log (log area))
             (/ (+ -0.35858 extent)
                (- area minor)))
          (* (+ (- 0.24140 major)
                (/ perimeter perimeter))
             (/ (exp extent)
                (* eccentricity eccentricity)))))
    (log (/ (/ (exp (log 0.13205))
               (exp (/ major eccentricity)))
            (- (/ (exp minor)
                  (/ eccentricity perimeter))
               (* (exp perimeter)
                  (* perimeter convex))))))
TREE-equ:
y = ((exp((exp((major + eccentricity)) - ((convex / perimeter) * (convex / eccentricity)))) + ((log(log(area)) / ((-0.35858 + extent) / (area - minor))) + (((0.24140 - major) + (perimeter / perimeter)) * (exp(extent) / (eccentricity * eccentricity))))) / log(((exp(log(0.13205)) / exp((major / eccentricity))) / ((exp(minor) / (eccentricity / perimeter)) - (exp(perimeter) * (perimeter * convex))))))
