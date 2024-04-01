=== BEST-OF-RUN ===
              generation: 0
                   nodes: 36
                   depth: 5
                    hits: 862
TOP INDIVIDUAL:



-- #1 --
                    hits: 862
             raw fitness: 862.0000
    standardized fitness: 862.0000
        adjusted fitness: 0.9988
TREE:
 (/ (- (* (exp (- area extent))
          (log (* extent minor)))
       (/ (exp (* extent perimeter))
          (log (+ convex eccentricity))))
    (* (exp (/ (+ eccentricity minor)
               (log eccentricity)))
       (log (/ (/ area convex)
               (- perimeter area)))))
TREE-equ:
y = (((exp((area - extent)) * log((extent * minor))) - (exp((extent * perimeter)) / log((convex + eccentricity)))) / (exp(((eccentricity + minor) / log(eccentricity))) * log(((area / convex) / (perimeter - area)))))
