# Microfmt

[![build](https://github.com/jeremy-rifkin/microfmt/actions/workflows/build.yml/badge.svg)](https://github.com/jeremy-rifkin/microfmt/actions/workflows/build.yml)
[![test](https://github.com/jeremy-rifkin/microfmt/actions/workflows/test.yml/badge.svg)](https://github.com/jeremy-rifkin/microfmt/actions/workflows/test.yml)
[![Quality Gate Status](https://sonarcloud.io/api/project_badges/measure?project=jeremy-rifkin_microfmt&metric=alert_status)](https://sonarcloud.io/summary/new_code?id=jeremy-rifkin_microfmt)

Microfmt is a very small (~300 line) formatting library. It's much more primitive than std::format or libfmt but it's
small and fast.

I haven't benchmarked against libfmt however I have found this library to be faster than std::format and more than twice
as fast as a stringstream.

It only supports string types and integers.

I wrote this because I needed basic formatting ability in two libraries but didn't want to pull in libfmt since that
would be an extra thing for downstream users who aren't using conan, vcpkg, or FetchContent to worry about.
