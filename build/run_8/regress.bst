=== BEST-OF-RUN ===
              generation: 7
                   nodes: 34
                   depth: 6
                    hits: 876
TOP INDIVIDUAL:



-- #1 --
                    hits: 876
             raw fitness: 876.0000
    standardized fitness: 876.0000
        adjusted fitness: 0.9989
TREE:
 (/ (/ (log (log (/ extent eccentricity)))
       (/ (/ (* eccentricity perimeter)
             (- major area))
          (- extent
             (* convex major))))
    (exp (* (log (- (log convex)
                    (exp major)))
            (+ (exp extent)
               (log (log perimeter))))))
TREE-equ:
y = ((log(log((extent / eccentricity))) / (((eccentricity * perimeter) / (major - area)) / (extent - (convex * major)))) / exp((log((log(convex) - exp(major))) * (exp(extent) + log(log(perimeter))))))
