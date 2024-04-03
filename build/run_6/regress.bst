=== BEST-OF-RUN ===
              generation: 6
                   nodes: 44
                   depth: 7
                    hits: 789
TOP INDIVIDUAL:



-- #1 --
                    hits: 789
             raw fitness: 789.0000
    standardized fitness: 789.0000
        adjusted fitness: 0.9987
TREE:
 (* (/ (/ (exp eccentricity) extent)
       (/ (* (exp perimeter)
             (* major -0.65856))
          (- (- 0.89693 eccentricity)
             (/ extent
                (* minor
                   (+ eccentricity eccentricity))))))
    (+ (+ (- (+ convex minor)
             (- area minor))
          (- (+ -0.42594 extent)
             (- minor -0.71593)))
       (log (/ area 0.40218))))
TREE-equ:
y = (((exp(eccentricity) / extent) / ((exp(perimeter) * (major * -0.65856)) / ((0.89693 - eccentricity) - (extent / (minor * (eccentricity + eccentricity)))))) * ((((convex + minor) - (area - minor)) + ((-0.42594 + extent) - (minor - -0.71593))) + log((area / 0.40218))))
