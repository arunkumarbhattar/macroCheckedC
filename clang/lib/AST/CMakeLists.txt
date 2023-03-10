set(LLVM_LINK_COMPONENTS
  BinaryFormat
  Core
  FrontendOpenMP
  Support
  )

# FIXME: the entry points to the interpreter should be moved out of clangAST
# into the parser or Sema in order to allow the interpreter to be moved to
# another library which depends on clangAST.
clang_tablegen(Opcodes.inc
  -gen-clang-opcodes
  SOURCE Interp/Opcodes.td
  TARGET Opcodes)

add_clang_library(clangAST
  AbstractSet.cpp
  APValue.cpp
  ASTConcept.cpp
  ASTConsumer.cpp
  ASTContext.cpp
  ASTDiagnostic.cpp
  ASTDumper.cpp
  ASTImporter.cpp
  ASTImporterLookupTable.cpp
  ASTStructuralEquivalence.cpp
  ASTTypeTraits.cpp
  AttrImpl.cpp
  CanonBounds.cpp
  CXXInheritance.cpp
  Comment.cpp
  CommentBriefParser.cpp
  CommentCommandTraits.cpp
  CommentLexer.cpp
  CommentParser.cpp
  CommentSema.cpp
  ComparisonCategories.cpp
  ComputeDependence.cpp
  CXXInheritance.cpp
  DataCollection.cpp
  Decl.cpp
  DeclarationName.cpp
  DeclBase.cpp
  DeclCXX.cpp
  DeclFriend.cpp
  DeclGroup.cpp
  DeclObjC.cpp
  DeclOpenMP.cpp
  DeclPrinter.cpp
  DeclTemplate.cpp
  ParentMapContext.cpp
  Expr.cpp
  ExprClassification.cpp
  ExprConcepts.cpp
  ExprConstant.cpp
  ExprCXX.cpp
  ExprObjC.cpp
  ExprUtils.cpp
  ExternalASTMerger.cpp
  ExternalASTSource.cpp
  FormatString.cpp
  InheritViz.cpp
  Interp/ByteCodeEmitter.cpp
  Interp/ByteCodeExprGen.cpp
  Interp/ByteCodeGenError.cpp
  Interp/ByteCodeStmtGen.cpp
  Interp/Context.cpp
  Interp/Descriptor.cpp
  Interp/Disasm.cpp
  Interp/EvalEmitter.cpp
  Interp/Frame.cpp
  Interp/Function.cpp
  Interp/Interp.cpp
  Interp/InterpBlock.cpp
  Interp/InterpFrame.cpp
  Interp/InterpStack.cpp
  Interp/InterpState.cpp
  Interp/Pointer.cpp
  Interp/PrimType.cpp
  Interp/Program.cpp
  Interp/Record.cpp
  Interp/Source.cpp
  Interp/State.cpp
  ItaniumCXXABI.cpp
  ItaniumMangle.cpp
  JSONNodeDumper.cpp
  Mangle.cpp
  MicrosoftCXXABI.cpp
  MicrosoftMangle.cpp
  NestedNameSpecifier.cpp
  NormalizeUtils.cpp
  NSAPI.cpp
  ODRHash.cpp
  OpenMPClause.cpp
  OSLog.cpp
  ParentMap.cpp
  PreorderAST.cpp
  PrintfFormatString.cpp
  QualTypeNames.cpp
  RawCommentList.cpp
  RecordLayout.cpp
  RecordLayoutBuilder.cpp
  ScanfFormatString.cpp
  SelectorLocationsKind.cpp
  Stmt.cpp
  StmtCXX.cpp
  StmtIterator.cpp
  StmtObjC.cpp
  StmtOpenMP.cpp
  StmtPrinter.cpp
  StmtProfile.cpp
  StmtViz.cpp
  TemplateBase.cpp
  TemplateName.cpp
  TextNodeDumper.cpp
  Type.cpp
  TypeLoc.cpp
  TypePrinter.cpp
  VTableBuilder.cpp
  VTTBuilder.cpp

  LINK_LIBS
  clangBasic
  clangLex

  DEPENDS
  Opcodes
  omp_gen
  )
