#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
DIR="$(mktemp -d)"
trap 'rm -rf "$DIR"' EXIT
for CXX in g++ clang++; do
  command -v "$CXX" >/dev/null || continue
  for ARCH in native avr; do
    ARGS=()
    if [ "$ARCH" = avr ]; then ARGS=(-D__AVR__); fi
    "$CXX" -std=c++11 -Wall -Wextra -Werror -pedantic -g -O1 -fsanitize=undefined       "${ARGS[@]}" -I"$ROOT/tests/fakes" -I"$ROOT/src"       "$ROOT/src/flprogNST1001.cpp" "$ROOT/tests/test_nst1001.cpp" -o "$DIR/test"
    "$DIR/test"
    echo "PASS: $CXX $ARCH UBSan"
  done
done
# Сборка штатного примера с заглушками Arduino, без Arduino IDE.
g++ -x c++ -std=c++11 -Wall -Wextra -Werror -pedantic -fsyntax-only   -I"$ROOT/tests/fakes" -I"$ROOT/src"   "$ROOT/examples/readTemperature/readTemperature.ino"
echo 'PASS: example C++11 syntax'
python3 - "$ROOT" <<'PY_KW'
import pathlib,sys,re
root=pathlib.Path(sys.argv[1]); rows=[]
for i,raw in enumerate((root/'keywords.txt').read_text().splitlines(),1):
 line=raw.strip()
 if not line or line.startswith('#'): continue
 v=raw.split('\t')
 assert len(v)==2 and v[1] in ('KEYWORD1','KEYWORD2','LITERAL1'), (i,raw)
 rows.append((v[0],v[1]))
assert len(rows)==len(set(x[0] for x in rows))==5
s=' '.join(f.read_text() for f in (root/'src').glob('*'))
for word,category in rows:
 assert re.search(r'\b'+re.escape(word)+r'\b',s), word
print('PASS: keywords.txt 5 unique valid entries')
PY_KW
