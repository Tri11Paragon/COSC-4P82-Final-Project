=== BEST-OF-RUN ===
              generation: 0
                   nodes: 68
                   depth: 6
                    hits: 552
TOP INDIVIDUAL:



-- #1 --
                    hits: 552
             raw fitness: 552.0000
    standardized fitness: 552.0000
        adjusted fitness: 0.9982
TREE:
 (* (/ (/ (- (* (/ perimeter eccentricity)
                (* perimeter major))
             (* (- perimeter perimeter)
                (- 0.16603 major)))
          (+ (+ (* convex minor)
                (- eccentricity major))
             (* (/ eccentricity major)
                (exp minor))))
       (log (+ (- (- major major)
                  (log major))
               (log (log eccentricity)))))
    (- (exp (log (+ (exp major)
                    (/ convex eccentricity))))
       (/ (exp (+ (exp major)
                  (* minor extent)))
          (log (/ (* -0.34321 perimeter)
                  (* eccentricity major))))))
TREE-equ:
y = ((((((perimeter / eccentricity) * (perimeter * major)) - ((perimeter - perimeter) * (0.16603 - major))) / (((convex * minor) + (eccentricity - major)) + ((eccentricity / major) * exp(minor)))) / log((((major - major) - log(major)) + log(log(eccentricity))))) * (exp(log((exp(major) + (convex / eccentricity)))) - (exp((exp(major) + (minor * extent))) / log(((-0.34321 * perimeter) / (eccentricity * major))))))
