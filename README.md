# 평범한 한글 C++
[평범한 한글](https://github.com/dragonteros/unsuspected-hangeul)이라는 난해한 한글 프로그래밍 언어의 C++ 구현체입니다.

## Implementation-defined
### 함수: 인수
- 곱셈 함수의 인수가 0개일 경우 0을 반환합니다.
- 곱셈 함수의 인수가 1개일 경우 인수를 그대로 반환합니다.
- 덧셈 함수의 인수가 0개일 경우 0을 반환합니다.
- 덧셈 함수의 인수가 1개일 경우 인수를 그대로 반환합니다.
- 거듭제곱 함수의 인수가 0개일 경우 0을 반환합니다.
- 거듭제곱 함수의 인수가 1개일 경우 인수를 그대로 반환합니다.
- 거듭제곱 함수의 인수가 3개 이상일 경우 첫번째, 두번째 이외의 인수는 무시합니다.
- 같음 함수의 인수가 2개 미만일 경우 True를 반환합니다.
- 같음 함수의 인수가 3개 이상일 경우 첫번째, 두번째 이외의 인수는 무시합니다.
- 작음 함수의 인수가 2개 미만일 경우 False를 반환합니다.
- 작음 함수의 인수가 3개 이상일 경우 첫번째, 두번째 이외의 인수는 무시합니다.
- 논리 부정 함수의 인수가 0개일 경우 True를 반환합니다.
- 논리 부정 함수의 인수가 2개 이상일 경우 첫번째 이외의 인수는 무시합니다.
- True 생성 함수의 인수는 무시합니다.
- False 생성 함수의 인수는 무시합니다.
- True 함수의 인수가 0개일 경우 True를 반환합니다.
- True 함수의 인수가 1개일 경우 인수를 그대로 반환합니다.
- True 함수의 인수가 3개 이상일 경우 첫번째 인수를 그대로 반환합니다.
- False 함수의 인수가 0개일 경우 False를 반환합니다.
- False 함수의 인수가 1개일 경우 인수를 그대로 반환합니다.
- False 함수의 인수가 3개 이상일 경우 두번째 인수를 그대로 반환합니다.
- 표준 입력의 경우 함수가 만들어진 시점이 아니라 값이 실제로 평가되는 시점에 호출합니다.

### 함수: 타입
- 곱셈, 덧셈, 거듭제곱 함수의 인수 중 실수 타입이 아닌 값이 포함되어 있을 경우 0을 반환합니다. 단, 논리 타입의 경우에는 False는 0, True는 1로 처리합니다.
- 같음, 작음 함수의 두 인수의 타입이 다를 경우 False를 반환합니다. 단, 실수 타입과 논리 타입을 비교하는 경우 논리 타입을 실수 타입으로 형 변환하여 비교하며, False는 0, True는 1로 처리합니다.
- 논리 부정 함수의 인수의 타입이 논리 타입이 아닐 경우 False를 반환합니다. 단, 실수 타입의 경우 0은 True, 그 외의 값은 False로 처리합니다.

### 인수 접근
- 올바르지 않은 인수에 접근할 경우(예: 음의 인덱스를 갖는 인수, 존재하지 않는 인수) False로 평가합니다.

## 라이선스
```
MIT License

Copyright (c) 2019 kmc7468

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```