# -*- encoding: utf-8 -*-
# stub: al_math 1.0.1 ruby lib

Gem::Specification.new do |s|
  s.name = "al_math".freeze
  s.version = "1.0.1".freeze

  s.required_rubygems_version = Gem::Requirement.new(">= 0".freeze) if s.respond_to? :required_rubygems_version=
  s.metadata = { "allowed_push_host" => "https://rubygems.org", "homepage_uri" => "https://github.com/al-org-dev/al-math", "source_code_uri" => "https://github.com/al-org-dev/al-math" } if s.respond_to? :metadata=
  s.require_paths = ["lib".freeze]
  s.authors = ["al-org".freeze]
  s.date = "1980-01-02"
  s.description = "Jekyll plugin extracted from al-folio that renders MathJax, pseudocode, and TikZJax styles/scripts through Liquid tags.".freeze
  s.email = ["dev@al-org.dev".freeze]
  s.homepage = "https://github.com/al-org-dev/al-math".freeze
  s.licenses = ["MIT".freeze]
  s.required_ruby_version = Gem::Requirement.new(">= 2.7".freeze)
  s.rubygems_version = "4.0.6".freeze
  s.summary = "Math rendering assets and tags for Jekyll".freeze

  s.installed_by_version = "3.6.7".freeze

  s.specification_version = 4

  s.add_runtime_dependency(%q<jekyll>.freeze, [">= 3.9".freeze, "< 5.0".freeze])
  s.add_runtime_dependency(%q<liquid>.freeze, [">= 4.0".freeze, "< 6.0".freeze])
  s.add_development_dependency(%q<bundler>.freeze, [">= 2.0".freeze, "< 3.0".freeze])
  s.add_development_dependency(%q<rake>.freeze, ["~> 13.0".freeze])
end
