=== BEST-OF-RUN ===
              generation: 6
                   nodes: 92
                   depth: 11
                    hits: 831
TOP INDIVIDUAL:



-- #1 --
                    hits: 831
             raw fitness: 831.0000
    standardized fitness: 831.0000
        adjusted fitness: 0.9988
TREE:
 (log (* (+ minor convex)
         (/ (exp (- (* (log extent)
                       (/ major extent))
                    (exp (log major))))
            (- (* (/ (/ convex area)
                     (* perimeter area))
                  (- (* minor extent)
                     (/ extent convex)))
               (/ (- (- (/ (* (+ (log eccentricity) extent)
                              (exp (* major eccentricity)))
                           (+ (exp (- -0.57977 -0.77457))
                              (exp (/ 0.53531 major))))
                        (+ (* (/ (exp perimeter)
                                 (+ area area))
                              (- (* -0.12475 perimeter)
                                 (log minor)))
                           (log (/ (/ area convex)
                                   (/ convex major)))))
                     (* minor area))
                  (+ (- (log 0.98118)
                        (+ area perimeter))
                     (/ (+ minor convex) perimeter)))))))
TREE-equ:
y = log(((minor + convex) * (exp(((log(extent) * (major / extent)) - exp(log(major)))) / ((((convex / area) / (perimeter * area)) * ((minor * extent) - (extent / convex))) - ((((((log(eccentricity) + extent) * exp((major * eccentricity))) / (exp((-0.57977 - -0.77457)) + exp((0.53531 / major)))) - (((exp(perimeter) / (area + area)) * ((-0.12475 * perimeter) - log(minor))) + log(((area / convex) / (convex / major))))) - (minor * area)) / ((log(0.98118) - (area + perimeter)) + ((minor + convex) / perimeter)))))))
