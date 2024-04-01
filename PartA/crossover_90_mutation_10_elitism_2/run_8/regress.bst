=== BEST-OF-RUN ===
              generation: 50
                   nodes: 117
                   depth: 14
                    hits: 0
TOP INDIVIDUAL:



-- #1 --
                    hits: 0
             raw fitness: 1501.6787
    standardized fitness: 1501.6787
        adjusted fitness: 0.0007
TREE:
 (+ (- (- (log (- (+ x
                     (+ (+ (exp (exp (- (+ x
                                           (* 0.09009 0.49060))
                                        (+ 0.03372
                                           (cos x)))))
                           (+ (sin 0.03372)
                              (cos (exp (+ (exp (+ x 0.69175)) 0.81505)))))
                        (cos (sin 0.25235))))
                  (- (* 0.69146 -0.86201)
                     (+ (+ (+ (exp (exp (+ (log x)
                                           (log x))))
                              (log (log (cos 0.07980))))
                           (sin (* (cos (+ x -0.99222))
                                   (cos (* x -0.62435))))) 0.81505))))
          (sin (exp (exp (+ (- (* 0.00559 0.20695)
                               (exp (+ 0.91897 x)))
                            (sin 0.25235))))))
       (sin (+ (+ (/ (+ (- 0.81664
                           (log x)) x)
                     (cos -0.06996))
                  (cos (* 0.03372 x))) 0.81505)))
    (+ (+ (exp (exp (+ (log x)
                       (sin 0.25235))))
          (+ (log (cos x))
             (/ (+ -0.33718 0.37605)
                (+ x x))))
       (exp x)))
TREE-equ:
y = (((log(((x + ((exp(exp(((x + (0.09009 * 0.49060)) - (0.03372 + cos(x))))) + (sin(0.03372) + cos(exp((exp((x + 0.69175)) + 0.81505))))) + cos(sin(0.25235)))) - ((0.69146 * -0.86201) - (((exp(exp((log(x) + log(x)))) + log(log(cos(0.07980)))) + sin((cos((x + -0.99222)) * cos((x * -0.62435))))) + 0.81505)))) - sin(exp(exp((((0.00559 * 0.20695) - exp((0.91897 + x))) + sin(0.25235)))))) - sin((((((0.81664 - log(x)) + x) / cos(-0.06996)) + cos((0.03372 * x))) + 0.81505))) + ((exp(exp((log(x) + sin(0.25235)))) + (log(cos(x)) + ((-0.33718 + 0.37605) / (x + x)))) + exp(x)))
