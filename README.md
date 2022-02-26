# libsched, a C++17 library for playing with scheduling problems

This library deals with the research around [optimal job scheduling](https://en.wikipedia.org/wiki/Optimal_job_scheduling).

It is composed of several parts:

- `common`: definitions common to all scheduling problems
- `para`: classes related to _single-stage job scheduling problems_ (P, Q, R), also known as parallel problems
- `shop`: classes related to _multi-stage job scheduling problems_ (O, F, J), also known as shop problems
- `tools`: various classes not related to scheduling but useful for computing things (`Matrix`, `Graph`, etc)


## Dependencies

- [{fmt}](https://github.com/fmtlib/fmt) (MIT)
- [nlohmann/json](https://github.com/nlohmann/json) (MIT)
