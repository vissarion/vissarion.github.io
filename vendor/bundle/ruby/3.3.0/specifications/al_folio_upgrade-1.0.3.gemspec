# -*- encoding: utf-8 -*-
# stub: al_folio_upgrade 1.0.3 ruby lib

Gem::Specification.new do |s|
  s.name = "al_folio_upgrade".freeze
  s.version = "1.0.3".freeze

  s.required_rubygems_version = Gem::Requirement.new(">= 0".freeze) if s.respond_to? :required_rubygems_version=
  s.metadata = { "allowed_push_host" => "https://rubygems.org", "homepage_uri" => "https://github.com/al-org-dev/al-folio-upgrade", "source_code_uri" => "https://github.com/al-org-dev/al-folio-upgrade" } if s.respond_to? :metadata=
  s.require_paths = ["lib".freeze]
  s.authors = ["al-folio maintainers".freeze]
  s.bindir = "exe".freeze
  s.date = "2026-05-25"
  s.description = "Provides audit, report, and safe codemod commands for al-folio upgrades.".freeze
  s.email = ["maintainers@al-folio.dev".freeze]
  s.executables = ["al-folio".freeze]
  s.files = ["exe/al-folio".freeze]
  s.homepage = "https://github.com/al-org-dev/al-folio-upgrade".freeze
  s.licenses = ["MIT".freeze]
  s.required_ruby_version = Gem::Requirement.new(">= 2.7".freeze)
  s.rubygems_version = "3.0.3.1".freeze
  s.summary = "Upgrade tooling for al-folio v1.x".freeze

  s.installed_by_version = "3.6.7".freeze

  s.specification_version = 4

  s.add_runtime_dependency(%q<jekyll>.freeze, [">= 3.9".freeze, "< 5.0".freeze])
  s.add_runtime_dependency(%q<liquid>.freeze, [">= 4.0".freeze, "< 6.0".freeze])
  s.add_runtime_dependency(%q<al_folio_core>.freeze, [">= 1.0.0".freeze])
  s.add_development_dependency(%q<bundler>.freeze, [">= 2.0".freeze, "< 3.0".freeze])
  s.add_development_dependency(%q<rake>.freeze, ["~> 13.0".freeze])
end
