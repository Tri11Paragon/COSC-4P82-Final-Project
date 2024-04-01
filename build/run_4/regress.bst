=== BEST-OF-RUN ===
              generation: 1
                   nodes: 79
                   depth: 6
                    hits: 939
TOP INDIVIDUAL:



-- #1 --
                    hits: 939
             raw fitness: 939.0000
    standardized fitness: 939.0000
        adjusted fitness: 0.9989
TREE:
 (/ (+ (exp (log (/ (* major eccentricity)
                    (- extent extent))))
       (- (- (- (* minor major)
                (- -0.52445 0.71341))
             (+ (+ perimeter convex)
                (* perimeter major)))
          (/ (* (exp area)
                (/ minor minor))
             (- (+ convex eccentricity)
                (- extent major)))))
    (* (exp (- (log (* area convex))
               (* (exp 0.63537)
                  (* eccentricity perimeter))))
       (- (+ (exp minor)
             (/ (log major)
                (+ convex eccentricity)))
          (- (/ (- perimeter perimeter)
                (/ 0.19608 major))
             (- (+ minor extent)
                (- convex perimeter))))))
TREE-equ:
y = ((exp(log(((major * eccentricity) / (extent - extent)))) + ((((minor * major) - (-0.52445 - 0.71341)) - ((perimeter + convex) + (perimeter * major))) - ((exp(area) * (minor / minor)) / ((convex + eccentricity) - (extent - major))))) / (exp((log((area * convex)) - (exp(0.63537) * (eccentricity * perimeter)))) * ((exp(minor) + (log(major) / (convex + eccentricity))) - (((perimeter - perimeter) / (0.19608 / major)) - ((minor + extent) - (convex - perimeter))))))
