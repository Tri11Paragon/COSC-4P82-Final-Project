=== BEST-OF-RUN ===
              generation: 29
                   nodes: 27
                   depth: 6
                    hits: 895
TOP INDIVIDUAL:



-- #1 --
                    hits: 895
             raw fitness: 895.0000
    standardized fitness: 895.0000
        adjusted fitness: 0.9989
TREE:
 (log (+ (log (/ (+ convex perimeter) extent))
         (/ (/ (+ (/ area perimeter)
                  (+ perimeter -0.43095))
               (log (/ major area)))
            (+ extent
               (log (/ area extent))))))
TREE-equ:
y = log((log(((convex + perimeter) / extent)) + ((((area / perimeter) + (perimeter + -0.43095)) / log((major / area))) / (extent + log((area / extent))))))
