=== BEST-OF-RUN ===
              generation: 0
                   nodes: 94
                   depth: 6
                    hits: 616
TOP INDIVIDUAL:



-- #1 --
                    hits: 616
             raw fitness: 616.0000
    standardized fitness: 616.0000
        adjusted fitness: 0.9984
TREE:
 (+ (+ (/ (log (+ (log major)
                  (/ 0.66193 major)))
          (/ (+ (+ area eccentricity)
                (* perimeter eccentricity))
             (/ (- major major)
                (log perimeter))))
       (+ (/ (* (- minor eccentricity)
                (+ major -0.00849))
             (/ (/ 0.33991 major)
                (+ extent convex)))
          (/ (exp (/ perimeter eccentricity))
             (- (/ extent extent)
                (* minor minor)))))
    (* (* (+ (+ (/ convex minor)
                (- convex minor))
             (+ (+ perimeter convex)
                (exp major)))
          (+ (- (/ minor extent)
                (/ extent minor))
             (* (/ 0.20455 minor)
                (exp perimeter))))
       (exp (- (- (log extent)
                  (* perimeter 0.95055))
               (log (/ minor extent))))))
TREE-equ:
y = (((log((log(major) + (0.66193 / major))) / (((area + eccentricity) + (perimeter * eccentricity)) / ((major - major) / log(perimeter)))) + ((((minor - eccentricity) * (major + -0.00849)) / ((0.33991 / major) / (extent + convex))) + (exp((perimeter / eccentricity)) / ((extent / extent) - (minor * minor))))) + (((((convex / minor) + (convex - minor)) + ((perimeter + convex) + exp(major))) * (((minor / extent) - (extent / minor)) + ((0.20455 / minor) * exp(perimeter)))) * exp(((log(extent) - (perimeter * 0.95055)) - log((minor / extent))))))
