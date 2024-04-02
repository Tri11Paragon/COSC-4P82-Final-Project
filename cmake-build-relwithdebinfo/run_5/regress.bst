=== BEST-OF-RUN ===
              generation: 0
                   nodes: 2
                   depth: 1
                    hits: 513
TOP INDIVIDUAL:



-- #1 --
                    hits: 513
             raw fitness: 513.0000
    standardized fitness: 513.0000
        adjusted fitness: 0.9981
TREE:
 (* (/ (exp (+ major minor))
       (- (* eccentricity major)
          (* 0.97781 minor)))
    (+ (exp (- eccentricity convex))
       (* (* 0.49033 minor)
          (log eccentricity))))
TREE-equ:
y = ((exp((major + minor)) / ((eccentricity * major) - (0.97781 * minor))) * (exp((eccentricity - convex)) + ((0.49033 * minor) * log(eccentricity))))
