=== BEST-OF-RUN ===
              generation: 0
                   nodes: 60
                   depth: 6
                    hits: 746
TOP INDIVIDUAL:



-- #1 --
                    hits: 746
             raw fitness: 746.0000
    standardized fitness: 746.0000
        adjusted fitness: 0.9987
TREE:
 (/ (* (log (/ (log (* eccentricity major))
               (log (- extent convex))))
       (* (+ (exp (/ area area))
             (- (log major)
                (+ major perimeter)))
          (exp (log (- extent minor)))))
    (- (+ (exp (* (- convex -0.26756)
                  (- eccentricity 0.89808)))
          (- (log (log convex))
             (log (exp extent))))
       (log (* (/ (exp perimeter)
                  (/ area minor))
               (* (+ minor extent)
                  (exp extent))))))
TREE-equ:
y = ((log((log((eccentricity * major)) / log((extent - convex)))) * ((exp((area / area)) + (log(major) - (major + perimeter))) * exp(log((extent - minor))))) / ((exp(((convex - -0.26756) * (eccentricity - 0.89808))) + (log(log(convex)) - log(exp(extent)))) - log(((exp(perimeter) / (area / minor)) * ((minor + extent) * exp(extent))))))
