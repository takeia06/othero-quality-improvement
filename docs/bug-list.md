# 既知不具合一覧

| ID | 状態 | 概要 | 影響 | 対応コミット |
|---|---|---|---|---|
| BUG-001 | 修正済み | `turn` の未初期化依存 | 手番管理が不安定になる | fix: correct turn switching and pass handling |
| BUG-002 | 修正済み | `canput` の範囲外アクセス危険 | 端や角で異常動作の可能性 | fix: add boundary checks for move search and neighbor scan |
| BUG-003 | 未対応 | 評価関数の条件式誤り | CPU評価が意図どおり動かない | - |
| BUG-004 | 未対応 | `display.h` の宣言不一致 | 保守性・分割時の整合性に問題 | - |
| BUG-005 | 修正予定 | 終局時に結果表示がすぐ閉じる | 検証しづらい | - |