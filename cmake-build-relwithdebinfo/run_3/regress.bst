=== BEST-OF-RUN ===
              generation: 0
                   nodes: 24
                   depth: 5
                    hits: 573
TOP INDIVIDUAL:



-- #1 --
                    hits: 573
             raw fitness: 573.0000
    standardized fitness: 573.0000
        adjusted fitness: 0.9983
TREE:
 (* (log (log (* (log eccentricity)
                 (exp eccentricity))))
    (log (- (* (+ major eccentricity)
               (+ perimeter major))
            (/ (/ major eccentricity)
               (/ perimeter convex)))))
TREE-equ:
y = (log(log((log(eccentricity) * exp(eccentricity)))) * log((((major + eccentricity) * (perimeter + major)) - ((major / eccentricity) / (perimeter / convex)))))
