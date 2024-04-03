=== BEST-OF-RUN ===
              generation: 9
                   nodes: 24
                   depth: 6
                    hits: 746
TOP INDIVIDUAL:



-- #1 --
                    hits: 746
             raw fitness: 746.0000
    standardized fitness: 746.0000
        adjusted fitness: 0.9987
TREE:
 (+ (* (* (log eccentricity) area)
       (log area))
    (+ (+ (+ convex 0.03015)
          (+ (/ (+ area minor) minor) major))
       (log (- area 0.32967))))
TREE-equ:
y = (((log(eccentricity) * area) * log(area)) + (((convex + 0.03015) + (((area + minor) / minor) + major)) + log((area - 0.32967))))
