# -*- encoding: utf-8 -*-
# stub: al_folio_bootstrap_compat 1.0.0 ruby lib

Gem::Specification.new do |s|
  s.name = "al_folio_bootstrap_compat".freeze
  s.version = "1.0.0".freeze

  s.required_rubygems_version = Gem::Requirement.new(">= 0".freeze) if s.respond_to? :required_rubygems_version=
  s.metadata = { "allowed_push_host" => "https://rubygems.org", "homepage_uri" => "https://github.com/al-org-dev/al-folio-bootstrap-compat", "source_code_uri" => "https://github.com/al-org-dev/al-folio-bootstrap-compat" } if s.respond_to? :metadata=
  s.require_paths = ["lib".freeze]
  s.authors = ["al-folio maintainers".freeze]
  s.date = "2026-02-16"
  s.description = "Adds runtime compatibility hooks and lifecycle warnings for Bootstrap-marked legacy content.".freeze
  s.email = ["maintainers@al-folio.dev".freeze]
  s.homepage = "https://github.com/al-org-dev/al-folio-bootstrap-compat".freeze
  s.licenses = ["MIT".freeze]
  s.required_ruby_version = Gem::Requirement.new(">= 2.7".freeze)
  s.rubygems_version = "3.0.3.1".freeze
  s.summary = "Optional Bootstrap compatibility layer for al-folio v1.x".freeze

  s.installed_by_version = "3.6.7".freeze

  s.specification_version = 4

  s.add_runtime_dependency(%q<jekyll>.freeze, [">= 3.9".freeze, "< 5.0".freeze])
  s.add_runtime_dependency(%q<liquid>.freeze, [">= 4.0".freeze, "< 6.0".freeze])
  s.add_development_dependency(%q<bundler>.freeze, [">= 2.0".freeze, "< 3.0".freeze])
  s.add_development_dependency(%q<rake>.freeze, ["~> 13.0".freeze])
end
