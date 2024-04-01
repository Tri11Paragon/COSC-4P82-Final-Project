=== BEST-OF-RUN ===
              generation: 0
                   nodes: 40
                   depth: 6
                    hits: 768
TOP INDIVIDUAL:



-- #1 --
                    hits: 768
             raw fitness: 768.0000
    standardized fitness: 768.0000
        adjusted fitness: 0.9987
TREE:
 (/ (* (+ (* (* (/ extent major)
                (- 0.35602 major))
             (/ (- perimeter area)
                (* area major)))
          (log (exp (/ convex area))))
       (log (* (log (log eccentricity))
               (log (log 0.19952)))))
    (log (log (exp (- (log major)
                      (+ -0.89115 minor))))))
TREE-equ:
y = ((((((extent / major) * (0.35602 - major)) * ((perimeter - area) / (area * major))) + log(exp((convex / area)))) * log((log(log(eccentricity)) * log(log(0.19952))))) / log(log(exp((log(major) - (-0.89115 + minor))))))
