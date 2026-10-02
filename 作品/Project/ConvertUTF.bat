@echo off
chcp 65001 > nul
echo 全ての .cpp と .h ファイルを UTF-8 (BOM付き) に変換します。
echo 対象フォルダ: %cd%
echo.

REM 作業用一時ファイル名
set TMPFILE=__temp_utf8_file__.txt

for /R %%f in (*.cpp *.h *.cs *.json) do (
    echo 変換中: %%f

    REM ファイルの内容を一時ファイルに書き出す（UTF-8 BOM付きで）
    powershell -Command "Get-Content -LiteralPath '%%f' | Out-File -FilePath '%TMPFILE%' -Encoding utf8"

    REM 元ファイルを上書き
    move /Y "%TMPFILE%" "%%f" > nul
)

echo.
echo 変換完了！
pause
