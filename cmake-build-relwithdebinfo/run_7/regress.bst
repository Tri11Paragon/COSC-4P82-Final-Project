=== BEST-OF-RUN ===
              generation: 14
                   nodes: 23
                   depth: 6
                    hits: 851
TOP INDIVIDUAL:



-- #1 --
                    hits: 851
             raw fitness: 851.0000
    standardized fitness: 851.0000
        adjusted fitness: 0.9988
TREE:
 (* (* (- extent
          (+ extent convex))
       (log (* extent perimeter)))
    (/ (+ minor eccentricity)
       (exp (+ (+ (+ major minor) extent) perimeter))))
TREE-equ:
y = (((extent - (extent + convex)) * log((extent * perimeter))) * ((minor + eccentricity) / exp((((major + minor) + extent) + perimeter))))
