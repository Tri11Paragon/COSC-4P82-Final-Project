=== BEST-OF-RUN ===
              generation: 15
                   nodes: 48
                   depth: 10
                    hits: 865
TOP INDIVIDUAL:



-- #1 --
                    hits: 865
             raw fitness: 865.0000
    standardized fitness: 865.0000
        adjusted fitness: 0.9988
TREE:
 (log (* (- (- major
               (- -0.87478 area))
            (/ (+ (* extent eccentricity)
                  (* area -0.35841))
               (exp (* perimeter eccentricity))))
         (/ (exp (log (- convex -0.18534)))
            (* (* (exp major)
                  (exp perimeter))
               (/ (/ (- (/ extent convex)
                        (- (- (exp minor)
                              (* convex eccentricity)) area)) eccentricity) -0.48653)))))
TREE-equ:
y = log((((major - (-0.87478 - area)) - (((extent * eccentricity) + (area * -0.35841)) / exp((perimeter * eccentricity)))) * (exp(log((convex - -0.18534))) / ((exp(major) * exp(perimeter)) * ((((extent / convex) - ((exp(minor) - (convex * eccentricity)) - area)) / eccentricity) / -0.48653)))))
