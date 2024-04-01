=== BEST-OF-RUN ===
              generation: 21
                   nodes: 41
                   depth: 9
                    hits: 923
TOP INDIVIDUAL:



-- #1 --
                    hits: 923
             raw fitness: 923.0000
    standardized fitness: 923.0000
        adjusted fitness: 0.9989
TREE:
 (* (- (- (/ 0.62145 minor) major)
       (- (exp perimeter)
          (exp -0.15746)))
    (exp (+ (+ (* (log (* convex
                          (log major)))
                  (log (- (- minor major)
                          (* (+ major -0.75304)
                             (+ minor area)))))
               (- (/ extent extent)
                  (+ perimeter perimeter))) 0.89914)))
TREE-equ:
y = ((((0.62145 / minor) - major) - (exp(perimeter) - exp(-0.15746))) * exp((((log((convex * log(major))) * log(((minor - major) - ((major + -0.75304) * (minor + area))))) + ((extent / extent) - (perimeter + perimeter))) + 0.89914)))
