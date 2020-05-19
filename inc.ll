; ModuleID = 'inc.bc'
source_filename = "inc.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @inc(i32 %a) #0 !dbg !7 {
entry:
  %a.addr = alloca i32, align 4
  store i32 %a, i32* %a.addr, align 4
  call void @llvm.dbg.declare(metadata i32* %a.addr, metadata !11, metadata !DIExpression()), !dbg !12
  %0 = load i32, i32* %a.addr, align 4, !dbg !13
  %add = add nsw i32 %0, 1, !dbg !14
  ret i32 %add, !dbg !15
}

; Function Attrs: nounwind readnone speculatable willreturn
declare void @llvm.dbg.declare(metadata, metadata, metadata) #1

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 !dbg !16 {
entry:
  %retval = alloca i32, align 4
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %z = alloca i32, align 4
  %flag = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @llvm.dbg.declare(metadata i32* %x, metadata !19, metadata !DIExpression()), !dbg !20
  call void @llvm.dbg.declare(metadata i32* %y, metadata !21, metadata !DIExpression()), !dbg !22
  call void @llvm.dbg.declare(metadata i32* %z, metadata !23, metadata !DIExpression()), !dbg !24
  call void @llvm.dbg.declare(metadata i32* %flag, metadata !25, metadata !DIExpression()), !dbg !26
  store i32 10, i32* %x, align 4, !dbg !27
  store i32 0, i32* %y, align 4, !dbg !28
  %0 = load i32, i32* %x, align 4, !dbg !29
  %1 = load i32, i32* %y, align 4, !dbg !30
  %add = add nsw i32 %0, %1, !dbg !31
  store i32 %add, i32* %z, align 4, !dbg !32
  %call = call i32 (i8*, ...) @__isoc99_scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32* %flag), !dbg !33
  %2 = load i32, i32* %flag, align 4, !dbg !34
  %cmp = icmp sgt i32 %2, 0, !dbg !36
  br i1 %cmp, label %if.then, label %if.else, !dbg !37

if.then:                                          ; preds = %entry
  %call1 = call i32 @inc(i32 0), !dbg !38
  store i32 %call1, i32* %x, align 4, !dbg !40
  %call2 = call i32 @inc(i32 -1), !dbg !41
  store i32 %call2, i32* %y, align 4, !dbg !42
  br label %if.end, !dbg !43

if.else:                                          ; preds = %entry
  %call3 = call i32 @inc(i32 -5), !dbg !44
  store i32 %call3, i32* %x, align 4, !dbg !46
  %call4 = call i32 @inc(i32 9), !dbg !47
  store i32 %call4, i32* %y, align 4, !dbg !48
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %3 = load i32, i32* %x, align 4, !dbg !49
  %4 = load i32, i32* %z, align 4, !dbg !50
  %add5 = add nsw i32 %3, %4, !dbg !51
  store i32 %add5, i32* %flag, align 4, !dbg !52
  ret i32 0, !dbg !53
}

declare dso_local i32 @__isoc99_scanf(i8*, ...) #2

attributes #0 = { noinline nounwind optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readnone speculatable willreturn }
attributes #2 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.dbg.cu = !{!0}
!llvm.module.flags = !{!3, !4, !5}
!llvm.ident = !{!6}

!0 = distinct !DICompileUnit(language: DW_LANG_C99, file: !1, producer: "clang version 10.0.0-++20200323042644+d32170dbd5b-1~exp1~20200323154014.129 ", isOptimized: false, runtimeVersion: 0, emissionKind: FullDebug, enums: !2, splitDebugInlining: false, nameTableKind: None)
!1 = !DIFile(filename: "inc.c", directory: "/tmp/llvm")
!2 = !{}
!3 = !{i32 7, !"Dwarf Version", i32 4}
!4 = !{i32 2, !"Debug Info Version", i32 3}
!5 = !{i32 1, !"wchar_size", i32 4}
!6 = !{!"clang version 10.0.0-++20200323042644+d32170dbd5b-1~exp1~20200323154014.129 "}
!7 = distinct !DISubprogram(name: "inc", scope: !1, file: !1, line: 7, type: !8, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!8 = !DISubroutineType(types: !9)
!9 = !{!10, !10}
!10 = !DIBasicType(name: "int", size: 32, encoding: DW_ATE_signed)
!11 = !DILocalVariable(name: "a", arg: 1, scope: !7, file: !1, line: 7, type: !10)
!12 = !DILocation(line: 7, column: 13, scope: !7)
!13 = !DILocation(line: 8, column: 12, scope: !7)
!14 = !DILocation(line: 8, column: 13, scope: !7)
!15 = !DILocation(line: 8, column: 5, scope: !7)
!16 = distinct !DISubprogram(name: "main", scope: !1, file: !1, line: 11, type: !17, scopeLine: 11, spFlags: DISPFlagDefinition, unit: !0, retainedNodes: !2)
!17 = !DISubroutineType(types: !18)
!18 = !{!10}
!19 = !DILocalVariable(name: "x", scope: !16, file: !1, line: 13, type: !10)
!20 = !DILocation(line: 13, column: 9, scope: !16)
!21 = !DILocalVariable(name: "y", scope: !16, file: !1, line: 13, type: !10)
!22 = !DILocation(line: 13, column: 12, scope: !16)
!23 = !DILocalVariable(name: "z", scope: !16, file: !1, line: 13, type: !10)
!24 = !DILocation(line: 13, column: 15, scope: !16)
!25 = !DILocalVariable(name: "flag", scope: !16, file: !1, line: 13, type: !10)
!26 = !DILocation(line: 13, column: 18, scope: !16)
!27 = !DILocation(line: 15, column: 7, scope: !16)
!28 = !DILocation(line: 16, column: 7, scope: !16)
!29 = !DILocation(line: 17, column: 9, scope: !16)
!30 = !DILocation(line: 17, column: 13, scope: !16)
!31 = !DILocation(line: 17, column: 11, scope: !16)
!32 = !DILocation(line: 17, column: 7, scope: !16)
!33 = !DILocation(line: 20, column: 5, scope: !16)
!34 = !DILocation(line: 23, column: 9, scope: !35)
!35 = distinct !DILexicalBlock(scope: !16, file: !1, line: 23, column: 9)
!36 = !DILocation(line: 23, column: 14, scope: !35)
!37 = !DILocation(line: 23, column: 9, scope: !16)
!38 = !DILocation(line: 24, column: 13, scope: !39)
!39 = distinct !DILexicalBlock(scope: !35, file: !1, line: 23, column: 19)
!40 = !DILocation(line: 24, column: 11, scope: !39)
!41 = !DILocation(line: 25, column: 13, scope: !39)
!42 = !DILocation(line: 25, column: 11, scope: !39)
!43 = !DILocation(line: 26, column: 5, scope: !39)
!44 = !DILocation(line: 27, column: 13, scope: !45)
!45 = distinct !DILexicalBlock(scope: !35, file: !1, line: 26, column: 12)
!46 = !DILocation(line: 27, column: 11, scope: !45)
!47 = !DILocation(line: 28, column: 13, scope: !45)
!48 = !DILocation(line: 28, column: 11, scope: !45)
!49 = !DILocation(line: 31, column: 12, scope: !16)
!50 = !DILocation(line: 31, column: 16, scope: !16)
!51 = !DILocation(line: 31, column: 14, scope: !16)
!52 = !DILocation(line: 31, column: 10, scope: !16)
!53 = !DILocation(line: 32, column: 5, scope: !16)
