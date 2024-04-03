=== BEST-OF-RUN ===
              generation: 13
                   nodes: 99
                   depth: 15
                    hits: 774
TOP INDIVIDUAL:



-- #1 --
                    hits: 774
             raw fitness: 774.0000
    standardized fitness: 774.0000
        adjusted fitness: 0.9987
TREE:
 (* (/ (* convex
          (* area extent)) extent)
    (/ (/ (- eccentricity minor)
          (+ area 0.06321))
       (/ (* (+ perimeter
                (* (exp eccentricity)
                   (+ (+ (- (exp (- (- perimeter 0.20147)
                                    (exp convex)))
                            (+ (/ (* (/ -0.21195 minor) convex)
                                  (+ perimeter area))
                               (log (exp major))))
                         (log (log (log (log (- (- (- -0.57336 eccentricity)
                                                   (* eccentricity major))
                                                (- (exp major) extent)))))))
                      (* (log (* (log (- eccentricity major))
                                 (/ -0.17648
                                    (- (* -0.83847 -0.09235)
                                       (/ area extent)))))
                         (exp (+ (log (- eccentricity minor))
                                 (- (log minor)
                                    (* major perimeter))))))))
             (+ (log eccentricity)
                (log area)))
          (/ (log major) convex))))
TREE-equ:
y = (((convex * (area * extent)) / extent) * (((eccentricity - minor) / (area + 0.06321)) / (((perimeter + (exp(eccentricity) * (((exp(((perimeter - 0.20147) - exp(convex))) - ((((-0.21195 / minor) * convex) / (perimeter + area)) + log(exp(major)))) + log(log(log(log((((-0.57336 - eccentricity) - (eccentricity * major)) - (exp(major) - extent))))))) + (log((log((eccentricity - major)) * (-0.17648 / ((-0.83847 * -0.09235) - (area / extent))))) * exp((log((eccentricity - minor)) + (log(minor) - (major * perimeter)))))))) * (log(eccentricity) + log(area))) / (log(major) / convex))))
