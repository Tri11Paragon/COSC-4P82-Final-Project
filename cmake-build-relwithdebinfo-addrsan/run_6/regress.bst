=== BEST-OF-RUN ===
              generation: 1
                   nodes: 60
                   depth: 6
                    hits: 595
TOP INDIVIDUAL:



-- #1 --
                    hits: 595
             raw fitness: 595.0000
    standardized fitness: 595.0000
        adjusted fitness: 0.9983
TREE:
 (- (+ (/ (/ (/ (log perimeter)
                (* area area))
             (/ (exp perimeter)
                (+ convex perimeter)))
          (- (/ (log eccentricity)
                (+ eccentricity convex))
             (+ (/ 0.23599 perimeter)
                (log minor))))
       (- (- (log (+ minor perimeter))
             (/ (+ area extent)
                (* convex eccentricity)))
          (+ (log (log extent))
             (exp (exp extent)))))
    (exp (* (/ extent
               (exp (* eccentricity eccentricity)))
            (- -0.62009 convex))))
TREE-equ:
y = (((((log(perimeter) / (area * area)) / (exp(perimeter) / (convex + perimeter))) / ((log(eccentricity) / (eccentricity + convex)) - ((0.23599 / perimeter) + log(minor)))) + ((log((minor + perimeter)) - ((area + extent) / (convex * eccentricity))) - (log(log(extent)) + exp(exp(extent))))) - exp(((extent / exp((eccentricity * eccentricity))) * (-0.62009 - convex))))
