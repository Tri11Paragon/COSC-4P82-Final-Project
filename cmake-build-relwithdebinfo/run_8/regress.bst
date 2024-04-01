=== BEST-OF-RUN ===
              generation: 0
                   nodes: 48
                   depth: 6
                    hits: 796
TOP INDIVIDUAL:



-- #1 --
                    hits: 796
             raw fitness: 796.0000
    standardized fitness: 796.0000
        adjusted fitness: 0.9987
TREE:
 (/ (- (exp (/ (log (log major))
               (- (* 0.74997 minor)
                  (/ perimeter 0.22278))))
       (log (* (- (- convex area)
                  (* area extent))
               (exp (- minor -0.78322)))))
    (log (* (+ (- (+ minor major)
                  (exp perimeter))
               (* (/ major -0.92937)
                  (+ perimeter -0.54531)))
            (exp (exp (/ perimeter minor))))))
TREE-equ:
y = ((exp((log(log(major)) / ((0.74997 * minor) - (perimeter / 0.22278)))) - log((((convex - area) - (area * extent)) * exp((minor - -0.78322))))) / log(((((minor + major) - exp(perimeter)) + ((major / -0.92937) * (perimeter + -0.54531))) * exp(exp((perimeter / minor))))))
