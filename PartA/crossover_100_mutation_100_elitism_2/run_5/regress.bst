=== BEST-OF-RUN ===
              generation: 33
                   nodes: 156
                   depth: 13
                    hits: 200
TOP INDIVIDUAL:



-- #1 --
                    hits: 200
             raw fitness: 0.6437
    standardized fitness: 0.6437
        adjusted fitness: 0.6084
TREE:
 (+ (+ x
       (- (* (* x x)
             (* x x))
          (- (sin (/ (- (sin x)
                        (sin x))
                     (/ (exp 0.97421) x)))
             (+ (log (cos (sin 0.08027)))
                (* (* x x)
                   (+ (/ x x) x))))))
    (* (* (log (- (exp x)
                  (* (* (sin (sin (sin (* 0.60474 0.59701))))
                        (- (cos 0.34951)
                           (+ (/ x
                                 (exp 0.12762))
                              (+ (- 0.33118
                                    (cos 0.17688))
                                 (/ (cos x)
                                    (log x))))))
                     (* 0.55794 0.68544))))
          (- x x))
       (- (cos (+ -0.49206
                  (+ (/ x
                        (exp 0.12762))
                     (+ (- 0.33118
                           (cos 0.17688))
                        (/ (cos x)
                           (log x))))))
          (* (log (* (* (cos (- 0.66241 x))
                        (- (- x x) x))
                     (+ -0.92982
                        (log x))))
             (exp (* (* (- x
                           (sin (/ (+ -0.60243
                                      (* (* x x) -0.01133))
                                   (+ (+ (sin 0.85234)
                                         (exp x)) x)))) x)
                     (cos (log (- (- (sin (sin (sin x)))
                                     (- x x))
                                  (+ (/ x
                                        (exp 0.12762))
                                     (+ x
                                        (/ (cos x)
                                           (log x)))))))))))))
TREE-equ:
y = ((x + (((x * x) * (x * x)) - (sin(((sin(x) - sin(x)) / (exp(0.97421) / x))) - (log(cos(sin(0.08027))) + ((x * x) * ((x / x) + x)))))) + ((log((exp(x) - ((sin(sin(sin((0.60474 * 0.59701)))) * (cos(0.34951) - ((x / exp(0.12762)) + ((0.33118 - cos(0.17688)) + (cos(x) / log(x)))))) * (0.55794 * 0.68544)))) * (x - x)) * (cos((-0.49206 + ((x / exp(0.12762)) + ((0.33118 - cos(0.17688)) + (cos(x) / log(x)))))) - (log(((cos((0.66241 - x)) * ((x - x) - x)) * (-0.92982 + log(x)))) * exp((((x - sin(((-0.60243 + ((x * x) * -0.01133)) / ((sin(0.85234) + exp(x)) + x)))) * x) * cos(log(((sin(sin(sin(x))) - (x - x)) - ((x / exp(0.12762)) + (x + (cos(x) / log(x)))))))))))))
