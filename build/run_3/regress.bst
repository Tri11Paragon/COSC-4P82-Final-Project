=== BEST-OF-RUN ===
              generation: 30
                   nodes: 49
                   depth: 8
                    hits: 911
TOP INDIVIDUAL:



-- #1 --
                    hits: 911
             raw fitness: 911.0000
    standardized fitness: 911.0000
        adjusted fitness: 0.9989
TREE:
 (/ (/ (- (log major)
          (log area))
       (* (/ (exp major)
             (log extent))
          (exp (+ 0.16844 perimeter))))
    (- (- (* (- area eccentricity)
             (- convex
                (log (* (exp 0.14565)
                        (+ minor perimeter)))))
          (+ (exp minor) major))
       (* (- (exp -0.05363)
             (* area area))
          (- (log 0.83699)
             (* -0.96176 major)))))
TREE-equ:
y = (((log(major) - log(area)) / ((exp(major) / log(extent)) * exp((0.16844 + perimeter)))) / ((((area - eccentricity) * (convex - log((exp(0.14565) * (minor + perimeter))))) - (exp(minor) + major)) - ((exp(-0.05363) - (area * area)) * (log(0.83699) - (-0.96176 * major)))))
